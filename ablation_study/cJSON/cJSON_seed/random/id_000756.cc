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
//<ID> 756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *city_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_city = 0;
    const char *version = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    city_item = cJSON_CreateString("Wonderland");

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_city = cJSON_AddItemToObject(root, "city", city_item);

    // step 4: Operate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    version = cJSON_Version();

    // step 5: Validate
    validation_score = (printed != (char *)0) + (int)added_name + (int)added_city + (version != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}