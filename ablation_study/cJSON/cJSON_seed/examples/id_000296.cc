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
//<ID> 296
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToArray(tags, cJSON_CreateString("engineer"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("team lead"));

    // step 2: Configure
    cJSON_AddItemToObjectCS(person, "name", name);
    cJSON_AddItemToObjectCS(person, "age", age);
    cJSON_AddItemToObjectCS(person, "tags", tags);
    cJSON_AddItemToObjectCS(root, "person", person);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON_AddItemToObjectCS(root, "has_person", cJSON_CreateBool(has_person));
    cJSON *got_person = cJSON_GetObjectItem(root, "person");
    cJSON *got_name = cJSON_GetObjectItem(got_person, "name");
    char *name_text = cJSON_GetStringValue(got_name);
    cJSON_AddItemToObjectCS(root, "name_copy", cJSON_CreateString(name_text));
    char *printed = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    cJSON_AddItemToObjectCS(root, "printed_ok", cJSON_CreateBool((cJSON_bool)(printed != NULL)));
    cJSON_bool person_has_name = cJSON_HasObjectItem(got_person, "name");
    cJSON_AddItemToObjectCS(root, "person_has_name", cJSON_CreateBool(person_has_name));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name_copy");
    cJSON_bool namecopy_exists_after_delete = cJSON_HasObjectItem(root, "name_copy");
    cJSON_AddItemToObjectCS(root, "namecopy_exists_after_delete", cJSON_CreateBool(namecopy_exists_after_delete));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}