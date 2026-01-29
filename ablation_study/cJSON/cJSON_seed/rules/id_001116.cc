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
//<ID> 1116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *people = NULL;
    cJSON *first_person = NULL;
    cJSON *name_obj = NULL;
    cJSON *greeting_obj = NULL;
    char *name_str = NULL;
    char *greeting_str = NULL;
    int people_count = 0;
    const char *json = "{\"people\":[{\"name\":\"Alice\"},{\"name\":\"Bob\"}],\"greeting\":\"hello\"}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    people = cJSON_GetObjectItem(root, "people");
    greeting_obj = cJSON_GetObjectItem(root, "greeting");

    // step 3: Operate / Validate
    people_count = cJSON_GetArraySize(people);
    first_person = cJSON_GetArrayItem(people, 0);
    name_obj = cJSON_GetObjectItem(first_person, "name");
    name_str = cJSON_GetStringValue(name_obj);
    greeting_str = cJSON_GetStringValue(greeting_obj);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}