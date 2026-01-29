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
//<ID> 1712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);

    // step 2: Configure
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, t);
    void *raw_buf = cJSON_malloc(32);
    char *buf = (char*)raw_buf;
    memset(buf, 0, 32);
    buf[0] = 'H';
    buf[1] = 'i';
    buf[2] = '!';
    cJSON *s = cJSON_CreateString(buf);
    cJSON_AddItemToArray(array, s);
    cJSON *n = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(array, n);

    // step 3: Operate and Validate
    cJSON_bool array_check1 = cJSON_IsArray(array);
    cJSON *retrieved = cJSON_GetObjectItem(root, "array");
    cJSON_bool array_check2 = cJSON_IsArray(retrieved);
    char *printed = cJSON_PrintUnformatted(root);
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    scratch[0] = printed ? printed[0] : 'X';
    scratch[1] = (char)('0' + (int)array_check1);
    scratch[2] = (char)('0' + (int)array_check2);
    scratch[3] = buf[0];
    (void)scratch;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(raw_buf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}