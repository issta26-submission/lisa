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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.23);
    cJSON *raw1 = cJSON_CreateRaw("initial_raw");
    const char *version = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(version);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "version", ver_ref);

    // step 3: Operate & Validate
    cJSON_bool raw_flag = cJSON_IsRaw(raw1);
    cJSON *raw_flag_bool = cJSON_CreateBool(raw_flag);
    cJSON_AddItemToObject(root, "raw_flag", raw_flag_bool);
    void *buf = cJSON_malloc((size_t)16);
    memset(buf, 0, 16);
    char *bufc = (char *)buf;
    bufc[0] = 'Z';
    bufc[1] = '\0';
    cJSON *buf_ref = cJSON_CreateStringReference(bufc);
    cJSON_AddItemToObject(root, "buffer_ref", buf_ref);
    cJSON *replacement = cJSON_CreateString("replacement");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *replaced_flag = cJSON_CreateBool(replaced);
    cJSON_AddItemToObject(root, "replaced", replaced_flag);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.89));

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buf);
    return 66; // API sequence test completed successfully
}