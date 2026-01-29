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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    const char *json = "{\"arr\": [10, 20], \"name\": \"orig\"}";
    const char *return_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *newnum = NULL;
    cJSON *nested = NULL;
    int size_before = 0;
    int size_after = 0;
    const char *name_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_ParseWithOpts(json, &return_end, 1);
    arr = cJSON_GetObjectItem(root, "arr");
    size_before = cJSON_GetArraySize(arr);
    newnum = cJSON_CreateNumber(30);
    cJSON_AddItemToArray(arr, newnum);
    cJSON_AddNumberToObject(root, "added_number", 3.14);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate / Validate
    size_after = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}