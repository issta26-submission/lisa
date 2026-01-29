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
//<ID> 1333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"person\": {\"age\": 30, \"name\": \"Alice\"}}";
    const char *unused_parse_end = NULL;
    cJSON *root = NULL;
    cJSON *person = NULL;
    cJSON *age_item = NULL;
    cJSON *name_item = NULL;
    cJSON *new_age_item = NULL;
    cJSON *new_name_item = NULL;
    cJSON *got_age_item = NULL;
    cJSON *got_name_item = NULL;
    double original_age = 0.0;
    double new_age_value = 0.0;
    const char *original_name = NULL;
    const char *new_name_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &unused_parse_end, 1);
    person = cJSON_GetObjectItem(root, "person");
    age_item = cJSON_GetObjectItem(person, "age");
    name_item = cJSON_GetObjectItem(person, "name");
    new_age_item = cJSON_CreateNumber(42.5);
    new_name_item = cJSON_CreateString("Bob");
    cJSON_AddItemToObject(root, "new_person_age", new_age_item);
    cJSON_AddItemToObject(root, "new_person_name", new_name_item);

    // step 3: Operate / Validate
    original_age = cJSON_GetNumberValue(age_item);
    original_name = cJSON_GetStringValue(name_item);
    got_age_item = cJSON_GetObjectItem(root, "new_person_age");
    got_name_item = cJSON_GetObjectItem(root, "new_person_name");
    new_age_value = cJSON_GetNumberValue(got_age_item);
    new_name_value = cJSON_GetStringValue(got_name_item);
    (void)original_name;
    (void)new_name_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}