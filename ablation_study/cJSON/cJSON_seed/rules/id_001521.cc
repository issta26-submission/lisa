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
//<ID> 1521
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
    cJSON *values = NULL;
    cJSON *count_item = NULL;
    cJSON *ver_ref = NULL;
    const char *version = NULL;
    int values_count = 0;
    double count_num = 0.0;
    char json[] = "{ \"name\": \"Tester\", \"values\": [10, 20, 30], \"meta\": { \"active\": true } }";
    size_t len = sizeof(json) - 1;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    version = cJSON_Version();
    ver_ref = cJSON_CreateStringReference(version);
    cJSON_AddItemToObject(root, "lib_version", ver_ref);

    // step 3: Operate / Validate
    values = cJSON_GetObjectItem(parsed, "values");
    values_count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(parsed, "values_count", (double)values_count);
    count_item = cJSON_GetObjectItem(parsed, "values_count");
    count_num = cJSON_GetNumberValue(count_item);
    (void)count_num;
    (void)scratch;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}