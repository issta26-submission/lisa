#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"flags\": [true, false, true], \"numbers\": [1.5, 2.5]}";
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *numbers = NULL;
    cJSON *flag0 = NULL;
    cJSON *flag1 = NULL;
    cJSON *flag2 = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    cJSON_bool is_flag0 = 0;
    cJSON_bool is_flag1 = 0;
    cJSON_bool is_flag2 = 0;
    char *buffer = NULL;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);
    flags = cJSON_GetObjectItem(root, "flags");
    numbers = cJSON_GetObjectItem(root, "numbers");
    buffer = (char *)cJSON_malloc((size_t)256);
    memset(buffer, 0, 256);

    // step 3: Operate / Validate
    flag0 = cJSON_GetArrayItem(flags, 0);
    flag1 = cJSON_GetArrayItem(flags, 1);
    flag2 = cJSON_GetArrayItem(flags, 2);
    is_flag0 = cJSON_IsTrue(flag0);
    is_flag1 = cJSON_IsTrue(flag1);
    is_flag2 = cJSON_IsTrue(flag2);
    num0 = cJSON_GetArrayItem(numbers, 0);
    num1 = cJSON_GetArrayItem(numbers, 1);
    val0 = cJSON_GetNumberValue(num0);
    val1 = cJSON_GetNumberValue(num1);
    sum = val0 + val1 + (double)is_flag0 + (double)is_flag1 + (double)is_flag2;
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)printed_ok;
    (void)sum;
    (void)is_flag0;
    (void)is_flag1;
    (void)is_flag2;
    // API sequence test completed successfully
    return 66;
}