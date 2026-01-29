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
//<ID> 796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("sensor");
    cJSON *greeting_item = cJSON_CreateString("Hello");
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON_AddItemToObject(person, "greeting", greeting_item);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snapshot_len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snapshot_len + 1);
    memset(buffer, 0, snapshot_len + 1);
    memcpy(buffer, snapshot, snapshot_len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_person_cs = cJSON_GetObjectItemCaseSensitive(parsed, "person");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_person_cs, "name");
    cJSON *parsed_greeting_cs = cJSON_GetObjectItemCaseSensitive(parsed_person_cs, "greeting");
    char *name_str = cJSON_GetStringValue(parsed_name);
    char *greet_str = cJSON_GetStringValue(parsed_greeting_cs);
    size_t comp_len = strlen(greet_str) + 2 + strlen(name_str) + 1;
    char *composed = (char *)cJSON_malloc(comp_len);
    memset(composed, 0, comp_len);
    sprintf(composed, "%s, %s", greet_str, name_str);
    cJSON *salutation = cJSON_CreateString(composed);
    cJSON_AddItemToObject(parsed, "salutation", salutation);
    cJSON *retrieved_sal = cJSON_GetObjectItem(parsed, "salutation");
    char *sal_value = cJSON_GetStringValue(retrieved_sal);
    (void)sal_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(composed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}