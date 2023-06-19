#include "stdio.h"
#include "unity.h"

void app_main(void)
{
	UNITY_BEGIN();
	unity_run_test_by_name("ADD of 3 and 4 is 7");
	UNITY_END();
}
