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
//<ID> 1502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObjectCS(root, "ints", int_array);
    cJSON_AddNumberToObject(meta, "count", 4.0);

    // step 2: Configure
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "ints_ref", int_array);
    cJSON_AddNumberToObject(meta, "ref_added", (double)ref_added);
    char *printed = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    size_t printed_len = printed ? strlen(printed) : 0;
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);

    // step 3: Operate & Validate
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int parsed_size = cJSON_GetArraySize(parsed_ints);
    cJSON *first_item = cJSON_GetArrayItem(parsed_ints, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    size_t consumed = parse_end ? (size_t)(parse_end - printed) : 0;
    cJSON_AddNumberToObject(dup, "consumed", (double)consumed);
    cJSON_AddNumberToObject(dup, "first_value", first_value);
    cJSON_bool equal = cJSON_Compare(parsed, dup, 1);
    (void)parsed_size;
    (void)equal;

    // step 4: Cleanup
    char *scratch = (char*)cJSON_malloc(16);
    memset(scratch, 0, 16);
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}