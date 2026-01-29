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
//<ID> 267
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    size_t buflen = 11;
    char *buf = (char*)cJSON_malloc(buflen);
    buf[0] = 'c'; buf[1] = 'u'; buf[2] = 's'; buf[3] = 't'; buf[4] = 'o'; buf[5] = 'm';
    buf[6] = '_'; buf[7] = 'r'; buf[8] = 'a'; buf[9] = 'w'; buf[10] = '\0';
    cJSON *raw = cJSON_CreateRaw(buf);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "data", arr);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObject(root, "version", ver_ref);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *replaced_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced", replaced_flag);
    cJSON *raw_item = cJSON_GetArrayItem(arr, 3);
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON *raw_flag = cJSON_CreateNumber((double)is_raw);
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buf);
    return 66;
    // API sequence test completed successfully
}