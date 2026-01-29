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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":10,\"beta\":[0.1,0.2]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "source", parsed);

    // step 2: Configure
    double values[4] = {3.14159, 2.71828, 1.61803, 0.57721};
    cJSON *dbl_array = cJSON_CreateDoubleArray(values, 4);
    cJSON_AddItemToObjectCS(root, "dbls", dbl_array);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON_bool has_source = cJSON_HasObjectItem(root, "source");
    cJSON_bool has_dbls = cJSON_HasObjectItem(root, "dbls");
    cJSON *fetched = cJSON_GetObjectItem(root, "dbls");
    cJSON *first_elem = cJSON_GetArrayItem(fetched, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    char *printed = cJSON_PrintUnformatted(root);
    (void)has_source;
    (void)has_dbls;
    (void)first_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}