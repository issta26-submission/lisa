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
//<ID> 220
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
    size_t bufsize = 16;
    char *buf = (char*)cJSON_malloc(bufsize);
    buf[0] = 'H'; buf[1] = 'i'; buf[2] = '\0';
    cJSON *greeting = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("configured"));
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    cJSON_AddItemToObject(root, "has_greeting", cJSON_CreateBool(has_greeting));
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "arr_size", cJSON_CreateNumber((double)arr_size));

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *outdup = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(outdup);
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}