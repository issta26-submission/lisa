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
//<ID> 751
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
    const char *json = "{\"name\":\"alpha\",\"arr\":[1,2]}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *subobj = NULL;
    cJSON *num_item = NULL;
    cJSON *added_number = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double got_pi = 0.0;
    const char *got_name = NULL;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_to_object = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithOpts(json, &parse_end, 1);
    subobj = cJSON_CreateObject();
    added_to_object = cJSON_AddItemToObject(root, "sub", subobj);
    added_number = cJSON_AddNumberToObject(root, "pi", 3.1415);
    arr = cJSON_GetObjectItem(root, "arr");
    num_item = cJSON_CreateNumber(42);
    added_to_array = cJSON_AddItemToArray(arr, num_item);
    added_to_object = cJSON_AddItemToObject(subobj, "inner", cJSON_CreateString("value"));
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_pi = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    got_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}