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
//<ID> 1519
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
    cJSON *person = NULL;
    cJSON *name = NULL;
    cJSON *salute = NULL;
    cJSON *name_ref = NULL;
    const char *parse_end = NULL;
    const char *name_val = NULL;
    const char *salute_val = NULL;
    char json[] = "{ \"person\": { \"name\": \"Bob\" }, \"age\": 30 }";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    salute = cJSON_CreateStringReference("Greetings");
    cJSON_AddItemToObject(parsed, "salute", salute);

    // step 3: Operate / Validate
    person = cJSON_GetObjectItem(parsed, "person");
    name = cJSON_GetObjectItem(person, "name");
    name_val = cJSON_GetStringValue(name);
    salute = cJSON_GetObjectItem(parsed, "salute");
    salute_val = cJSON_GetStringValue(salute);
    name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON *retrieved = cJSON_GetObjectItem(root, "name_ref");
    char *retrieved_val = cJSON_GetStringValue(retrieved);
    (void)salute_val;
    (void)retrieved_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}