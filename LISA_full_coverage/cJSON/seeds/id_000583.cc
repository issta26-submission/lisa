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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30 }, \"tags\": [\"dev\",\"cjson\"] }";
    cJSON *root = cJSON_Parse(raw_json);
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    char *name_value = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *nickname = cJSON_CreateString("Ally");
    cJSON_AddItemToObject(person, "nickname", nickname);
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    double age_value = cJSON_GetNumberValue(age_item);

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int pre_len = 512;
    char *prebuf = (char *)cJSON_malloc(pre_len);
    memset(prebuf, 0, pre_len);
    cJSON_PrintPreallocated(root, prebuf, pre_len, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "tags");

    // step 4: Validate & Cleanup
    (void)age_value;
    size_t name_len = name_value ? strlen(name_value) : 0;
    (void)name_len;
    cJSON_free(buffered);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}