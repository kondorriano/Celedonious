//LOL HIGHLIGHT Y TODO (copiado del qt, docs no es tan awesome)

#ifndef TASKPOOL_HPP
#define TASKPOOL_HPP

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>


class TaskPool {
	public:
		TaskPool(size_t threads, std::function<void()> = [](){});
		~TaskPool();

		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>;

		void wait();
		void discard();

	private:
		std::vector< std::thread > workers;
		std::queue< std::function<void()> > tasks;

		std::mutex queue_mutex;
		std::condition_variable condition;
		std::condition_variable endtasks;
		bool stop;
};

inline TaskPool::TaskPool(size_t threads, std::function<void()> init) : stop(false) {
	for(size_t i = 0;i<threads;++i)
		workers.emplace_back( [this, init] {
			init();
			for(;;) {
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				while(!this->stop && this->tasks.empty()) {
					this->endtasks.notify_all();
					this->condition.wait(lock);
				}
				if(this->stop && this->tasks.empty()) {
					this->endtasks.notify_all();
					return;
				}
				std::function<void()> task(this->tasks.front());
				this->tasks.pop();
				lock.unlock();
				task();
			}
		});
}

inline TaskPool::~TaskPool() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for(size_t i = 0;i<workers.size();++i)
		workers[i].join();
}

inline void TaskPool::wait() {
	std::unique_lock<std::mutex> lock(this->queue_mutex);
	while(!tasks.empty()) {
		this->endtasks.wait(lock);
	}
}


template<class F, class... Args>
auto TaskPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
	typedef typename std::result_of<F(Args...)>::type return_type;

	if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");
	auto task = std::make_shared< std::packaged_task<return_type()> >(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
					);
	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tasks.push([task](){ (*task)(); });
	}

	condition.notify_one();
	return res;
}

inline void TaskPool::discard() {
	std::unique_lock<std::mutex> lock(queue_mutex);
	tasks = std::queue< std::function<void()> >();
}

#endif //TASKPOOL_HPP
