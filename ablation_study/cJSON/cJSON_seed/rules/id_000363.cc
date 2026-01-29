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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *json = (char *)cJSON_malloc(128);
    memset(json, 0, 128);
    const char *literal = "{\"original\": 10, \"value\": 3.14}";
    memcpy(json, literal, strlen(literal) + 1);
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    cJSON *tmp = NULL;
    double retrieved_val = 0.0;
    cJSON_bool add_ok = 0;

    // step 2: Setup / Configure
    add_ok = cJSON_AddItemToObject(root, "source", parsed);
    cJSON *placed = cJSON_AddNumberToObject(root, "computed", 42.0);
    add_ok = cJSON_AddItemToObject(root, "extra", cJSON_CreateNumber(7.0));

    // step 3: Operate / Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "computed");
    cJSON *readded = cJSON_AddNumberToObject(root, "computed", 84.0);
    tmp = cJSON_GetObjectItem(root, "computed");
    retrieved_val = cJSON_GetNumberValue(tmp);
    buffer[0] = (char)('0' + (int)(retrieved_val == 84.0));
    buffer[1] = (char)('0' + (int)cJSON_IsNumber(tmp));
    (void)placed;
    (void)add_ok;

    // step 4: Cleanup
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}