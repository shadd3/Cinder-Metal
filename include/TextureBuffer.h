//
//  TextureBuffer.hpp
//  MetalCube
//
//  Created by William Lindmeier on 10/30/15.
//
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"
#include "MetalHelpers.hpp"
#include "MetalEnums.h"

namespace cinder { namespace mtl {
    
    typedef std::shared_ptr<class TextureBuffer> TextureBufferRef;
    
    // http://metalbyexample.com/textures-and-samplers/
    
    class TextureBuffer
    {
        
        friend class ImageSourceTextureBuffer;
        
    public:
        
        struct Format
        {
            Format() :
            mMipmapLevel(1)
            ,mSampleCount(1)
            ,mTextureType(TextureType2D)
            ,mPixelFormat(PixelFormatInvalid)
            ,mDepth(1)
            ,mArrayLength(1)
            ,mUsage(TextureUsageShaderRead)
            {};
            
            FORMAT_OPTION(mipmapLevel, MipmapLevel, int)
            FORMAT_OPTION(sampleCount, SampleCount, int)
            FORMAT_OPTION(textureType, TextureType, TextureType)
            FORMAT_OPTION(pixelFormat, PixelFormat, PixelFormat)
            FORMAT_OPTION(depth, Depth, int)
            FORMAT_OPTION(arrayLength, ArrayLength, int)
            FORMAT_OPTION(usage, Usage, TextureUsage)
        };
        
        static TextureBufferRef create( const ImageSourceRef & imageSource, const Format & format = Format() )
        {
            return TextureBufferRef( new TextureBuffer( imageSource, format ) );
        }

        static TextureBufferRef create( uint width, uint height, const Format & format = Format() )
        {
            return TextureBufferRef( new TextureBuffer( width, height, format ) );
        }
        
        static TextureBufferRef create( void * mtlTexture )
        {
            return TextureBufferRef( new TextureBuffer( mtlTexture ) );
        }

        virtual ~TextureBuffer();

        void update( const ImageSourceRef & imageSource );
        
        // Getting & Setting Data for 2D images
        void setPixelData( const void *pixelBytes );
        void getPixelData( void *pixelBytes );
        
        ci::ImageSourceRef createSource();

        // Accessors
        Format      getFormat() const;
        long		getWidth() const;
        long		getHeight() const;
        long		getDepth() const;
        ci::ivec2	getSize() const { return ivec2( getWidth(), getHeight() ); }
        long        getMipmapLevelCount();
        long        getSampleCount();
        long        getArrayLength();
        bool        getFramebufferOnly();
        TextureUsage getUsage(); // <MTLTextureUsage>
        
        void getBytes( void * pixelBytes, const ivec3 regionOrigin, const ivec3 regionSize,
                      uint bytesPerRow, uint bytesPerImage, uint mipmapLevel = 0, uint slice = 0);
        
        void replaceRegion( const ivec3 regionOrigin, const ivec3 regionSize, const void * newBytes,
                           uint bytesPerRow, uint bytesPerImage, uint mipmapLevel = 0, uint slice = 0 );
        
        TextureBufferRef newTexture( PixelFormat pixelFormat, TextureType type, uint levelOffset = 0,
                                    uint levelLength = 1, uint sliceOffset = 0, uint sliceLength = 1 );
        
        void *      getNative(){ return mImpl; };

    protected:

        TextureBuffer( const ImageSourceRef & imageSource, Format format );
        TextureBuffer( uint width, uint height, Format format );
        TextureBuffer( void * mtlTexture );

        void updateWidthCGImage( void * );
        void generateMipmap();
        
        void *mImpl = NULL; // <MTLTexture>
        long mBytesPerRow;
        Format mFormat;
        
        ImageIo::ChannelOrder mChannelOrder;
        ImageIo::ColorModel mColorModel;
        ImageIo::DataType mDataType;

    };
    
} }