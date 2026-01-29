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
//<ID> 666
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "name", "fuzzer");
    cJSON *arr1 = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr1);
    cJSON_AddItemToArray(arr1, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr1, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr1, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *letters = cJSON_AddArrayToObject(root, "letters");
    cJSON_AddItemToArray(letters, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(letters, cJSON_CreateString("beta"));
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);

    // step 3: Operate and Validate
    (void)cJSON_PrintPreallocated(root, prealloc, 512, 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_bool name_is_string = cJSON_IsString(cJSON_GetObjectItem(meta, "name"));
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);
    cJSON_AddStringToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_free(prealloc);
    // API sequence test completed successfully
    return 66;
}