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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"sensor\":\"temp\",\"values\":[1,2,3]}";
    size_t json_len = sizeof("{\"sensor\":\"temp\",\"values\":[1,2,3]}") - 1;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *created_number = cJSON_CreateNumber(123.0);
    cJSON *retrieved_added = NULL;
    double retrieved_value = 0.0;
    cJSON *helper_added = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(values, created_number);
    helper_added = cJSON_AddNumberToObject(root, "added_by_helper", 456.0);

    // step 3: Operate / Validate
    retrieved_added = cJSON_GetObjectItem(root, "added_by_helper");
    retrieved_value = cJSON_GetNumberValue(retrieved_added);
    printed = cJSON_PrintUnformatted(root);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + ((int)retrieved_value % 10));
    buffer[1] = (char)('A' + ((int)retrieved_value % 26));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}