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
//<ID> 1501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[4] = {10, 20, 30, 40};
    cJSON *arr = cJSON_CreateIntArray(numbers, 4);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_bool added_ref = cJSON_AddItemReferenceToObject(root, "values_ref", arr);
    (void)added_ref;

    // step 2: Configure
    const char json_literal[] = "{\"id\":123,\"meta\":{\"flag\":true}}";
    size_t json_len = sizeof(json_literal) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_literal, json_len, &parse_end, 1);
    cJSON_AddItemToObjectCS(root, "parsed", parsed);

    // step 3: Operate & Validate
    int array_size = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *id_item = cJSON_GetObjectItem(parsed, "id");
    double id_value = cJSON_GetNumberValue(id_item);
    (void)array_size;
    (void)second_value;
    (void)id_value;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}