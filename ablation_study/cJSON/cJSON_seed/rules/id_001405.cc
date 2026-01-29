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
//<ID> 1405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *person = NULL;
    cJSON *colors = NULL;
    char buffer[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool add_ref_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "first", "Alice");
    cJSON_AddStringToObject(person, "last", "Smith");
    colors = cJSON_CreateArray();
    cJSON_AddItemToArray(colors, cJSON_CreateString("red"));
    cJSON_AddItemToArray(colors, cJSON_CreateString("green"));
    cJSON_AddItemToArray(colors, cJSON_CreateString("blue"));
    cJSON_AddItemToObject(person, "favorite_colors", colors);
    cJSON_AddItemToObject(root, "person", person);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "person_alias", person);

    // step 3: Operate / Validate
    memset(buffer, 0, sizeof(buffer));
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    (void)add_ref_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}