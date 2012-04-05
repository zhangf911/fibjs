#include "ifs/fs.h"
#include "File.h"
#include <string.h>
#include <exlib/thread.h>
#include <exlib/lockfree.h>
#include "AsyncCall.h"

namespace fibjs
{

extern v8::Isolate* isolate;

result_t fs_base::open(const char* fname, const char* mode,
		obj_ptr<File_base>& retVal)
{
	obj_ptr<File> pFile = new File;
	result_t hr;

	hr = pFile->Open(fname, mode);
	if (hr < 0)
		return hr;

	retVal = pFile;

	return 0;
}

result_t fs_base::create(const char* fname, bool Overwrite,
		obj_ptr<File_base>& retVal)
{
	return 0;
}

result_t fs_base::tmpFile(obj_ptr<File_base>& retVal)
{
	retVal = new (File);
	return 0;
}

static result_t real_readFile(const char* fname, std::string& retVal)
{
	FILE* file = fopen(fname, "rb");
	if (file == NULL)
		return LastError();

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	std::string buf;

	buf.resize(size);

	for (int i = 0; i < size;)
	{
		int read = static_cast<int> (fread(&buf[i], 1, size - i, file));
		i += read;
	}
	fclose(file);

	retVal = buf;

	return 0;
}

extern AsyncQueue s_acPool;

result_t fs_base::readFile(const char* fname, std::string& retVal)
{
	v8::Unlocker unlocker(isolate);
	return AryncCallProc(s_acPool, real_readFile, fname, retVal);
}

result_t fs_base::writeFile(const char* fname, const char* txt)
{
	FILE* file = fopen(fname, "wb+");
	if (file == NULL)
		return LastError();

	int size = (int) strlen(txt);

	fwrite(txt, 1, size, file);
	fclose(file);

	return 0;
}

}
