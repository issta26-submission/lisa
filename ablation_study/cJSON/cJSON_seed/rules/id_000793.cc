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
//<ID> 793
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\": [1, 2, 3]}";
    const char *ver_str = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    cJSON *version_item = NULL;
    cJSON *count_item = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(parsed, "arr");
    arr_size = cJSON_GetArraySize(arr);
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw("[4,5]");
    cJSON_AddItemToObject(root, "raw_fragment", raw_item);
    ver_str = cJSON_Version();
    version_item = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate / Validate
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}