#ifndef CELEDONIOUSPROFILER_HPP
#define CELEDONIOUSPROFILER_HPP
#include "commons.hpp"

class CeledoniousProfiler final : public Profiler {
	public:
		CeledoniousProfiler();
		~CeledoniousProfiler();

		//cosas que el resto de la aplicación seteará
		static vec2f playerPos;
		static int particleCount;
		static float waterCounter;

	private:
		void renderCustomInterface() const override;
};

#endif // CELEDONIOUSPROFILER_HPP
