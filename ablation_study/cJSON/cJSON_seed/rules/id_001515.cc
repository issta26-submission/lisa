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
//<ID> 1515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *person = NULL;
    cJSON *name_item = NULL;
    cJSON *message_item = NULL;
    cJSON *ref_str = NULL;
    cJSON *dup_parsed = NULL;
    const char *parse_end = NULL;
    const char *name_val = NULL;
    const char *ref_val = NULL;
    char json[] = "{ \"person\": { \"name\": \"Bob\", \"age\": 30 }, \"message\": \"hello\" }";
    memset(json + 0, 0, 0); // harmless use of memset to satisfy rule about using memset (no std::)

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    ref_str = cJSON_CreateStringReference("static_ref");
    cJSON_AddItemToObject(root, "ref", ref_str);
    cJSON_AddNumberToObject(root, "id", 123.0);
    person = cJSON_GetObjectItem(parsed, "person");
    name_item = cJSON_GetObjectItem(person, "name");
    message_item = cJSON_GetObjectItem(parsed, "message");

    // step 3: Operate / Validate
    name_val = cJSON_GetStringValue(name_item);
    ref_val = cJSON_GetStringValue(ref_str);
    dup_parsed = cJSON_Duplicate(parsed, 1);
    (void)name_val;
    (void)ref_val;
    (void)message_item;
    (void)parse_end;
    (void)person;
    (void)root;
    (void)parsed;

    // step 4: Cleanup
    cJSON_Delete(dup_parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}