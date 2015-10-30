//
//  GeomTarget.hpp
//  MetalCube
//
//  Created by William Lindmeier on 10/24/15.
//
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/GeomIo.h"
#include "MetalGeom.h"
#include "DataBuffer.h"
#include "RenderEncoder.h"

namespace cinder
{
    namespace mtl
    {
        typedef std::shared_ptr<class VertexBuffer> VertexBufferRef;
        
        class VertexBuffer : public ci::geom::Target
        {
            
        public:

            // Create a container with no data.
            // Data should be passed in with setBufferForAttribute().
            static VertexBufferRef create( const ci::geom::AttribSet & requestedAttribs,
                                           ci::mtl::geom::Primitive primitive = ci::mtl::geom::TRIANGLE );

            // NOTE: The order that the attribs are passed in is the order that they
            // should appear in the shader.
            static VertexBufferRef create( const ci::geom::Source & source,
                                           const ci::geom::AttribSet & requestedAttribs );
            virtual ~VertexBuffer(){}
            
            ci::mtl::geom::Primitive getPrimitive(){ return mPrimitive; };
            void setPrimitive( const ci::mtl::geom::Primitive primitive ){ mPrimitive = primitive; };
            
            void setBufferForAttribute( DataBufferRef buffer, const ci::geom::Attrib attr );
            DataBufferRef getBufferForAttribute( const ci::geom::Attrib attr );
            
            template<typename T>
            void update( ci::geom::Attrib attr, std::vector<T> vectorData )
            {
                getBufferForAttribute(attr)->update(vectorData);
            }
            
            void setVertexLength( size_t vertLength ){ mVertexLength = vertLength; };
            size_t getVertexLength(){ return mVertexLength; };

            void render( RenderEncoderRef renderEncoder );
            void render( RenderEncoderRef renderEncoder, size_t vertexLength, size_t vertexStart = 0, size_t instanceCount = 1 );
            
        protected:
            
            VertexBuffer( const ci::geom::Source & source, const ci::geom::AttribSet &requestedAttribs );
            VertexBuffer( const ci::geom::AttribSet &requestedAttribs, ci::mtl::geom::Primitive primitive );

            // geom::Target subclass
            // Only use internally
            void copyAttrib( ci::geom::Attrib attr, uint8_t dims, size_t strideBytes, const float *srcData, size_t count );
            void copyIndices( ci::geom::Primitive primitive, const uint32_t *source, size_t numIndices, uint8_t requiredBytesPerIndex );
            uint8_t getAttribDims( ci::geom::Attrib attr ) const;

            ci::mtl::geom::Primitive mPrimitive;
            std::map< ci::geom::Attrib, DataBufferRef > mAttributeBuffers;
            ci::geom::AttribSet mRequestedAttribs;
            DataBufferRef mIndexBuffer;
            ci::geom::SourceRef mSource;
            size_t mVertexLength;
            
        };
    }
}
