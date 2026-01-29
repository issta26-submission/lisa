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
//<ID> 1508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 4);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(child, "ints", int_arr);

    // step 2: Configure
    const char json[] = "{\"ref\":{\"a\":10}}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *parsed_ref = cJSON_GetObjectItem(parsed, "ref");
    cJSON_AddItemReferenceToObject(root, "external_ref", parsed_ref);
    cJSON_AddItemToObjectCS(root, "childCase", child);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *third_item = cJSON_GetArrayItem(int_arr, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    (void)third_value;
    (void)parse_end;

    // step 4: Cleanup
    cJSON *detached = cJSON_DetachItemFromObject(root, "external_ref");
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    (void)detached;
    return 66;
    // API sequence test completed successfully
}