#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication1/stdafx.h"
#include <stdarg.h>
#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		void out(const char* fmt, ...)
		{
			using std::string;
			string strResult;
			char str_buf[1024];
			if (NULL != fmt)
			{
				va_list marker = NULL;
				va_start(marker, fmt); //初始化变量参数  
				size_t nLength = _vscprintf(fmt, marker) + 1; //获取格式化字符串长度  
				std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组  
				int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
				if (nWritten > 0)
				{
					strResult = &vBuffer[0];
				}
				va_end(marker); //重置变量参数  
			}
			if (strResult.empty())
			{
				return;
			}
			strcpy_s(str_buf, strResult.c_str());
			Logger::WriteMessage(str_buf);
		}
		
		TEST_METHOD(TestPush1)
		{
			Stack* stack = new Stack();
			int item = 0;
			bool real_value = stack->push(item);
			bool expected_value = true;
			Assert::AreEqual(real_value, expected_value);

			for (int i = 1; i <= stack->size(); i++) {

			}
		}

		TEST_METHOD(TestPushPop2)
		{
			Stack* stack = new Stack();
			printf("TestPushPop2 started\n");
			for (int i = 1; i <= stack->size()+1; i++) {
				int item = 1;
				bool real_value = stack->push(item);
				bool expected_value = i <= stack->size();
				out("push %d", i);
				Assert::AreEqual(real_value, expected_value);
			}
			// stack is full now
			for (int i = 1; i <= stack->size()+1; i++) {
				int pop_item;
				bool real_value = stack->pop(pop_item);
				bool expected_value = i <= stack->size();
				out("pop %d", i);
				Assert::AreEqual(real_value, expected_value);
			}
		}

		TEST_METHOD(MatrixTest)
		{
			int mat_input[9][9] =
			{
				1, 2, 3, 0, 0, 0, 0, 0, 0,
				4, 5, 6, 0, 0, 0, 0, 0, 0,
				7, 8, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0
			};
			Matrix matrix;
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					Change c = Change(i + 1, j + 1, mat_input[i][j]);
					matrix.fill_in(c);
					int real_value = matrix.get_point_value(i + 1, j + 1);
					int expected_value = mat_input[i][j];
					Assert::AreEqual(real_value, expected_value);
				}
			}
			{
				int real_value = matrix.set_point_candi(3, 3);
				int expected_value = 1;
				out("candi count:%d", real_value);
				Assert::AreEqual(real_value, expected_value);
			}
			{
				int real_value = matrix.set_point_candi(9, 9);
				int expected_value = 9;
				out("candi count:%d", real_value);
				Assert::AreEqual(real_value, expected_value);
			}
			{
				int real_value = matrix.set_point_candi(4, 1);
				int expected_value = 6;
				out("candi count:%d", real_value);
				Assert::AreEqual(real_value, expected_value);
			}
		}
	};
}