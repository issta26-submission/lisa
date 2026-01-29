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
//<ID> 652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    double initial_numbers[3] = {1.1, 2.2, 3.3};
    cJSON *numbers_array = cJSON_CreateDoubleArray(initial_numbers, 3);
    cJSON_AddItemToObject(root, "numbers", numbers_array);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 2: Configure
    char *unformatted = cJSON_PrintUnformatted(root);
    size_t buf_len = strlen(unformatted);
    char *buffer = (char *)cJSON_malloc(buf_len + 1);
    memset(buffer, 0, buf_len + 1);
    sprintf(buffer, "%s", unformatted);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    double replacement_numbers[2] = {9.9, 8.8};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_numbers, 2);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "numbers", replacement_array);
    int replacement_count = cJSON_GetArraySize(replacement_array);
    (void)version;
    (void)parsed_count;
    (void)replaced;
    (void)replacement_count;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}