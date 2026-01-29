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
//<ID> 1329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [\"one\",\"two\",\"three\"]}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup = NULL;
    cJSON_bool eq = 0;
    const char *ver = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddStringToObject(root, "added_note", "added_via_cJSON");
    cJSON_AddNumberToObject(root, "arr_size_num", (double)arr_size);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(arr, 1);
    eq = cJSON_Compare(arr, dup, 1);
    ver = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", ver);
    (void)eq;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}