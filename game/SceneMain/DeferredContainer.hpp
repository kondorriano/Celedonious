#ifndef DEFERREDCONTAINER_HPP
#define DEFERREDCONTAINER_HPP
#include "commons.hpp"

class DeferredContainer : public ContainerObject {
	public:

		enum DrawMode {
			Deferred = 0,
			Light,
			Forward
		};

		DeferredContainer();
		~DeferredContainer();

		void update(float deltaTime);
		void draw() const;
		DrawMode getMode() const;
		Texture2D* getColor0() const;
		Texture2D* getColor1() const;
		Texture2D* getDepth() const;

	private:
		Texture2D gBufferColor0;
		Texture2D gBufferColor1;
		Texture2D gBufferDepth;
		RenderTarget* gBuffer;
		mutable DrawMode drawMode;
		MeshBase* quad;
};

#endif // DEFERREDCONTAINER_HPP
