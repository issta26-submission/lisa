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
//<ID> 1507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int values[4] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(values, 4);
    cJSON_AddItemToObjectCS(root, "numbers", int_arr);

    // step 2: Configure & Operate
    cJSON_AddItemReferenceToObject(root, "numbers_ref", int_arr);
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Validate
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, printed ? (size_t)strlen(printed) : 0, &parse_end, 1);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int original_size = cJSON_GetArraySize(int_arr);
    int parsed_size = cJSON_GetArraySize(parsed_numbers);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)parse_end;
    (void)original_size;
    (void)parsed_size;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}