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
//<ID> 1525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *obj = NULL;
    cJSON *size_item = NULL;
    cJSON *k_item = NULL;
    const char *ver = NULL;
    const char *kval = NULL;
    char json[] = "{ \"arr\": [10, 20, 30], \"obj\": { \"k\": \"v\" } }";
    size_t len = sizeof(json) - 1;
    int arr_size = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    arr = cJSON_GetObjectItem(parsed, "arr");
    obj = cJSON_GetObjectItem(parsed, "obj");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_item);
    k_item = cJSON_GetObjectItem(obj, "k");
    kval = cJSON_GetStringValue(k_item);
    cJSON_AddStringToObject(root, "k_value", kval);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}