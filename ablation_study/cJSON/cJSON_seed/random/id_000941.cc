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
//<ID> 941
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_count = "count";
    const char *key_pi = "pi";
    const char *key_name = "name";
    const char *name_val = "example_name";
    cJSON *root = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    double val_count = 0.0;
    double val_pi = 0.0;
    const char *got_name = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create items and attach using both AddItemToObject and AddItemToObjectCS
    count_item = cJSON_CreateNumber(42.0);
    add_ok1 = cJSON_AddItemToObject(root, key_count, count_item);
    pi_item = cJSON_CreateNumber(3.14159);
    add_ok2 = cJSON_AddItemToObjectCS(root, key_pi, pi_item);
    name_item = cJSON_CreateString(name_val);
    add_ok3 = cJSON_AddItemToObjectCS(root, key_name, name_item);

    // step 4: Operate - retrieve items and extract values using GetNumberValue and GetStringValue
    count_item = cJSON_GetObjectItem(root, key_count);
    val_count = cJSON_GetNumberValue(count_item);
    pi_item = cJSON_GetObjectItem(root, key_pi);
    val_pi = cJSON_GetNumberValue(pi_item);
    name_item = cJSON_GetObjectItem(root, key_name);
    got_name = cJSON_GetStringValue(name_item);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(val_count == 42.0) + (int)(val_pi > 3.0) + (int)(got_name != (const char *)0);
    (void)validation_score;
    (void)val_count;
    (void)val_pi;
    (void)got_name;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}