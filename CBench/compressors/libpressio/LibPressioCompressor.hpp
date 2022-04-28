#ifdef CBENCH_HAS_LIBPRESSIO

#ifndef _LIBPRESSIO_COMPRESSOR_H_
#define _LIBPRESSIO_COMPRESSOR_H_

#include <sstream>
#include "compressorInterface.hpp"
#include "timer.hpp"
#include "libpressio_ext/cpp/libpressio.h"
#include "pressio_data.h"
#include "libpressio_ext/json/pressio_options_json.h"

class LibPressioCompressor: public CompressorInterface
{

  public:
    LibPressioCompressor();
    ~LibPressioCompressor();

    void init();
    int compress(void *input, void *&output, std::string dataType, size_t dataTypeSize, size_t * n);
    int decompress(void *&input, void *&output, std::string dataType, size_t dataTypeSize, size_t * n);
    void close();
};

inline LibPressioCompressor::LibPressioCompressor()
{
    compressorName = "LibPressio";
}

inline LibPressioCompressor::~LibPressioCompressor()
{

}


inline void LibPressioCompressor::init()
{

}

inline int LibPressioCompressor::compress(void *input, void *&output, std::string dataType, size_t dataTypeSize, size_t * n)
{
  pressio library;
  auto id = compressorParameters.find("libpressio_compressor_id")
  pressio_compressor compressor = library.get_compressor(id);

  std::string config_file_path = compressorParameters.find("libpressio_compressor_config");
  std::string config;
  std::ifstream infile(config_file_path);
  infile >> config_file_path;

  pressio_options* options = pressio_options_new_json(&library, config.c_str());
  compressor->set_options(*options);

  std::vector<size_t> dims(n, n+5); //TODO Verify this number
  auto input_data = pressio_data::nonowning(pressio_float_dtype, input, dims)
  //auto input_data = pressio_data::nonowning(pressio_byte_dtype, input, dims)
  auto compressed_data = pressio_data::empty(pressio_float_dtype, {})
  compressor->compress(input_data, compressed_data);
  //compressor->decompress(compressed_data, decompressed_data);

  output = pressio_data_copy(&compressed_data);
  *n = compressed_data->size_in_bytes();



  ///

	size_t numel = n[0];
	for (int i = 1; i < 5; i++)
		if (n[i] != 0)
			numel *= n[i];

	Timer cTime; cTime.start();
	SZ_Init(NULL);

	int mode = PW_REL; // Default by Sheng, PW_REL = 10
	std::string _mode = "PW_REL";

	double relTol = 0.0;
	std::unordered_map<std::string, std::string>::const_iterator got = compressorParameters.find("rel");
	if( got != compressorParameters.end() )
		if (compressorParameters["rel"] != "")
		{
			relTol = strConvert::to_double(compressorParameters["rel"]);
			mode = REL;
			_mode = "REL";
		}

	double absTol = 0.0;
	got = compressorParameters.find("abs");
	if( got != compressorParameters.end() )
		if (compressorParameters["abs"] != "")
		{
			absTol = strConvert::to_double(compressorParameters["abs"]);
			mode = ABS;
			_mode = "ABS";
		}

	double powerTol = 0.0;
	got = compressorParameters.find("pw_rel");
	if( got != compressorParameters.end() )
		if (compressorParameters["pw_rel"] != "")
		{
			powerTol = strConvert::to_double(compressorParameters["pw_rel"]);
			mode = PW_REL;
			_mode = "PW_REL";
			// Unknown mode, just fill in input to SZ
		}

	std::size_t csize = 0;
	std::uint8_t *cdata = SZ_compress_args(SZ_FLOAT, static_cast<float *>(input), &csize, mode, absTol, relTol, powerTol, n[4], n[3], n[2], n[1], n[0]);

	output = cdata;
	cTime.stop();

	cbytes = csize;

	log << "\n" << compressorName << " ~ InputBytes: " << dataTypeSize*numel << ", OutputBytes: " << csize << ", cRatio: " << (dataTypeSize*numel / (float)csize) << ", #elements: " << numel << std::endl;
	log << " ~ Mode used: " << _mode << " abs: " << absTol << ", rel: " << relTol << ", pw_tol: " << powerTol << " val: " << n[4] << ", " << n[3] << ", " << n[2] << ", " <<n[1] << ", " << n[0] << std::endl;
	log << compressorName << " ~ CompressTime: " << cTime.getDuration() << " s " << std::endl;

	return 1;
}


inline int LibPressioCompressor::decompress(void *&input, void *&output, std::string dataType, size_t dataTypeSize, size_t * n)
{
	size_t numel = n[0];
	for (int i = 1; i < 5; i++)
		if (n[i] != 0)
			numel *= n[i];

	Timer dTime; dTime.start();
	output = SZ_decompress(SZ_FLOAT, static_cast<std::uint8_t *>(input), cbytes, n[4], n[3], n[2], n[1], n[0]);
	dTime.stop();

	std::free(input);	input=NULL;

	log << compressorName << " ~ DecompressTime: " << dTime.getDuration() << " s " << std::endl;

	return 1;
}

inline void LibPressioCompressor::close()
{
	SZ_Finalize();
}

#endif // _SZ_COMPRESSOR_H_
#endif // CBENCH_HAS_SZ

