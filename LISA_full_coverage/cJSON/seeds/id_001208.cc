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
//<ID> 1208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateStringReference("one");
    cJSON *s2 = cJSON_CreateStringReference("two");
    cJSON *s3 = cJSON_CreateStringReference("three");

    // step 2: Configure
    cJSON_bool ok1 = cJSON_InsertItemInArray(arr, 0, s1);
    cJSON_bool ok2 = cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_bool ok3 = cJSON_InsertItemInArray(arr, 1, s3); // insert s3 between s1 and s2

    // step 3: Operate
    char *printed = cJSON_Print(arr);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(arr, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    int size = cJSON_GetArraySize(arr);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    const char *val0 = cJSON_GetStringValue(item0);
    (void)ok1;
    (void)ok2;
    (void)ok3;
    (void)prealloc_ok;
    (void)size;
    (void)val0;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}