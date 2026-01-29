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
//<ID> 1329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "username", "fuzzer_user");
    double numbers[3] = {3.14, 2.718, 1.618};
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *note_item = cJSON_AddStringToObject(root, "note", "array_of_doubles");

    // step 3: Operate and Validate
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "username");
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool is_array = cJSON_IsArray(retrieved_values);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(80);
    memset(scratch, 0, 80);
    const char *name_str = cJSON_GetStringValue(retrieved_name);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = name_str ? name_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (is_array ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}