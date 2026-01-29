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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_data = NULL;
    cJSON *constructed = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    const char *verstr = NULL;
    int parsed_size = 0;
    int constructed_size = 0;
    char json[] = "{ \"data\": [100, 200, 300], \"meta\": { \"id\": 42 } }";
    size_t len = sizeof(json) - 1;
    char prebuf[64];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    constructed = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10);
    n2 = cJSON_CreateNumber(20);
    cJSON_AddItemToArray(constructed, n1);
    cJSON_AddItemToArray(constructed, n2);
    cJSON_AddItemToObject(root, "constructed", constructed);
    verstr = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", verstr);

    // step 3: Operate / Validate
    parsed_data = cJSON_GetObjectItem(parsed, "data");
    parsed_size = cJSON_GetArraySize(parsed_data);
    cJSON_AddNumberToObject(root, "parsed_data_size", (double)parsed_size);
    constructed_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "constructed"));
    cJSON_AddNumberToObject(root, "constructed_size", (double)constructed_size);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}