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
//<ID> 982
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{ \"arr\": [10, 20, 30], \"flag\": false }";
    size_t raw_len = (size_t)(sizeof(raw_json) - 1);
    char *minify_buf = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *status_false = NULL;
    int local_ints[] = {7, 8, 9};
    cJSON *nums_array = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *first_item = NULL;
    double first_value = 0.0;
    cJSON *first_copy = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    int scratch_len = 16;

    // step 2: Setup / Configure
    minify_buf = (char *)cJSON_malloc(raw_len + 1);
    memcpy(minify_buf, raw_json, raw_len + 1);
    cJSON_Minify(minify_buf);
    parsed = cJSON_Parse(minify_buf);
    root = cJSON_CreateObject();
    status_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", status_false);
    nums_array = cJSON_CreateIntArray(local_ints, 3);
    cJSON_AddItemToObject(root, "local_numbers", nums_array);

    // step 3: Operate / Validate
    parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    first_item = cJSON_GetArrayItem(parsed_arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    first_copy = cJSON_CreateNumber(first_value);
    cJSON_AddItemToObject(root, "first_from_parsed", first_copy);
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = (char)('0' + ((int)first_value % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_free(minify_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}