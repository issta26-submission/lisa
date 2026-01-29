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
//<ID> 1505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObjectCS(root, "numbers", ints);
    cJSON_AddStringToObject(root, "description", "an integer array");
    cJSON_AddItemReferenceToObject(root, "numbers_ref", ints);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);

    // step 3: Operate & Validate
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON *first_item = cJSON_GetArrayItem(parsed_numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)parsed_count;
    (void)first_value;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}