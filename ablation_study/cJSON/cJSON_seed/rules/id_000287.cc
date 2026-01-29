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
//<ID> 287
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
    cJSON *hobbies = cJSON_CreateArray();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    char *printed = NULL;
    int hobbies_count = 0;
    cJSON *got_person = NULL;
    cJSON *age_item = NULL;
    double age_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON_AddItemToObject(person, "hobbies", hobbies);
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("coding"));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    hobbies_count = cJSON_GetArraySize(hobbies);
    got_person = cJSON_GetObjectItem(root, "person");
    age_item = cJSON_GetObjectItem(got_person, "age");
    age_value = cJSON_GetNumberValue(age_item);
    buffer[0] = (char)('0' + (hobbies_count % 10));
    (void)age_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}