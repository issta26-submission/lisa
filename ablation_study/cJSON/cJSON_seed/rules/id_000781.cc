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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\": [1, 2, 3], \"obj\": {\"a\": 10}}";
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *new_num = NULL;
    cJSON *added_num = NULL;
    cJSON *replaced_item = NULL;
    char *printed = NULL;
    double replaced_value = 0.0;
    double verify_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(parsed, "arr");
    new_num = cJSON_CreateNumber(42.5);
    cJSON_ReplaceItemInArray(arr, 1, new_num);
    added_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemToObject(parsed, "added", added_num);

    // step 3: Operate / Validate
    replaced_item = cJSON_GetArrayItem(arr, 1);
    replaced_value = cJSON_GetNumberValue(replaced_item);
    verify_value = replaced_value + cJSON_GetNumberValue(added_num);
    printed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}