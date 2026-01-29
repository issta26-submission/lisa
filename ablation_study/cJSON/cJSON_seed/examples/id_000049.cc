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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"existing\":true}";
    cJSON *root = cJSON_Parse(initial_json);

    // step 2: Configure
    double numbers[] = {1.5, 2.5, 3.5};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "doubles", double_array);
    cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    cJSON_bool has_existing = cJSON_HasObjectItem(root, "existing");
    double status_vals[2];
    status_vals[0] = (double)has_existing;
    status_vals[1] = (double)has_doubles;
    cJSON *status_array = cJSON_CreateDoubleArray(status_vals, 2);
    cJSON_AddItemToObjectCS(root, "status", status_array);
    cJSON *imported = cJSON_Parse("{\"imported\":42}");
    cJSON_AddItemToObjectCS(root, "import", imported);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}