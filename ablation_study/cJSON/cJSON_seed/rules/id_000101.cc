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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *names = cJSON_CreateArray();
    cJSON *first = cJSON_CreateString("Alice");
    cJSON *last = cJSON_CreateString("Smith");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *active = cJSON_CreateTrue();
    char *temp_buf = (char *)cJSON_malloc(8);
    char *printed = NULL;
    cJSON *got_person = NULL;
    cJSON *got_names = NULL;
    cJSON *first_name = NULL;
    cJSON_bool is_str = 0;
    int name_count = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(names, first);
    cJSON_AddItemToArray(names, last);
    cJSON_AddItemToObject(person, "names", names);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "active", active);
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Operate / Validate
    got_person = cJSON_GetObjectItem(root, "person");
    got_names = cJSON_GetObjectItemCaseSensitive(got_person, "names");
    first_name = cJSON_GetArrayItem(got_names, 0);
    is_str = cJSON_IsString(first_name);
    name_count = cJSON_GetArraySize(got_names);
    printed = cJSON_PrintUnformatted(root);
    temp_buf[0] = printed[0];
    temp_buf[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(temp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}