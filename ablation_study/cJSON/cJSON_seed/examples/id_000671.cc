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
//<ID> 671
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
    cJSON *values = cJSON_CreateArray();
    cJSON *numA = cJSON_CreateNumber(1.23);
    cJSON *numRef = cJSON_CreateNumber(9.0);

    // step 2: Configure
    cJSON_AddItemToArray(values, numA);
    cJSON_AddItemToObject(root, "primary", numRef);
    cJSON_AddItemReferenceToArray(values, numRef);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "total", 2.0);
    double set_result = cJSON_SetNumberHelper(numA, 99.99);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_primary = cJSON_GetObjectItemCaseSensitive(parsed, "primary");
    double primary_val = cJSON_GetNumberValue(parsed_primary);
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    (void)version;
    (void)set_result;
    (void)primary_val;
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}