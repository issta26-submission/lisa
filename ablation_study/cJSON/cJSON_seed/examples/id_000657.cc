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
//<ID> 657
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
    double values[3] = {3.14, 2.718, 1.618};
    cJSON *num_array = cJSON_CreateDoubleArray(values, 3);
    cJSON *status = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON_AddItemToObject(root, "status", status);
    int original_count = cJSON_GetArraySize(num_array);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s", snapshot);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("updated");
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "status", replacement);
    char *final_repr = cJSON_Print(parsed);

    // step 4: Cleanup
    (void)version;
    (void)original_count;
    (void)parsed_count;
    cJSON_free(snapshot);
    cJSON_free(final_repr);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}