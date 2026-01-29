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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int size_values = 0;
    cJSON *found_meta = NULL;
    cJSON *found_name = NULL;
    const char *name_str = NULL;
    double version_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddNumberToObject(meta, "version", 3.1415);
    cJSON_AddStringToObject(meta, "name", "cJSON_example");

    // step 3: Operate / Validate
    size_values = cJSON_GetArraySize(values);
    found_meta = cJSON_GetObjectItem(root, "meta");
    found_name = cJSON_GetObjectItem(found_meta, "name");
    name_str = cJSON_GetStringValue(found_name);
    version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(found_meta, "version"));
    buffer[0] = (char)('A' + (size_values % 26));
    (void)name_str;
    (void)version_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}