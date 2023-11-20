#pragma once
#include <iostream>
#include <string>

namespace GeoFrame {
	struct GeoFrameException : public std::exception {
	private:
		std::string mMessage = "EXCEPTION THROWED.";

	public:
		GeoFrameException(std::string msg) : mMessage(msg) { ; }
		const char* what() const noexcept { return mMessage.c_str(); }
	};

	struct FatalError : public GeoFrameException {
	public:
		FatalError(std::string msg) : GeoFrameException(msg) { ; }
	};


	struct IndexOutOfRangeError : public GeoFrameException {
	public:
		IndexOutOfRangeError(std::string msg) : GeoFrameException(msg) { ; }
	};


	struct ShaderTypeError : public GeoFrameException {
	public:
		ShaderTypeError(std::string msg) : GeoFrameException(msg) { ; }
	};

	struct ShaderNotCompletedError : public GeoFrameException {
	public:
		ShaderNotCompletedError(std::string msg) : GeoFrameException(msg) { ; }
	};


	struct FontInitializationError : public GeoFrameException {
	public:
		FontInitializationError(std::string msg) : GeoFrameException(msg) { ; }
	};

	struct FontLoadingError : public GeoFrameException {
	public:
		FontLoadingError(std::string msg) : GeoFrameException(msg) { ; }
	};


	struct IndexBufferError : public GeoFrameException {
	public:
		IndexBufferError(std::string msg) : GeoFrameException(msg) { ; }
	};

	struct InvalidFormatError : public GeoFrameException {
	public:
		InvalidFormatError(std::string msg) : GeoFrameException(msg) { ; }
	};
	

	struct FramebufferError : public GeoFrameException {
	public:
		FramebufferError(std::string msg) : GeoFrameException(msg) { ; }
	};
}