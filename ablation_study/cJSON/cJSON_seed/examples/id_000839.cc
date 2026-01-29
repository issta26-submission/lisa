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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_numbers[3] = { 1.5, 2.5, 3.5 };
    cJSON *values = cJSON_CreateDoubleArray(initial_numbers, 3);
    cJSON *payload = cJSON_CreateRaw("<<BINARY_PAYLOAD>>");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "values", values);
    cJSON_AddItemToObjectCS(root, "payload", payload);
    cJSON *replacement = cJSON_CreateRaw("\"replaced_raw\"");
    cJSON_ReplaceItemInArray(values, 1, replacement);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *first_elem = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *computed = cJSON_CreateNumber(first_val + 10.0);
    cJSON_AddItemToObjectCS(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}