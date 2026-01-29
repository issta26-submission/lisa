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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\":[10,20,30],\"name\":\"orig\"}";
    cJSON *root = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *added_str = (cJSON *)0;
    cJSON *fetched_added = (cJSON *)0;
    int array_size = 0;
    double second_val = 0.0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into cJSON structure
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain the array and read a numeric value
    numbers = cJSON_GetObjectItem(root, "numbers");
    array_size = cJSON_GetArraySize(numbers);
    second_item = cJSON_GetArrayItem(numbers, 1);
    second_val = cJSON_GetNumberValue(second_item);

    // step 4: Operate - create a new string node and add it to the root using CS API
    added_str = cJSON_CreateString("injected");
    add_ok = cJSON_AddItemToObjectCS(root, "injectedKey", added_str);
    fetched_added = cJSON_GetObjectItem(root, "injectedKey");

    // step 5: Validate - compute a small validation score exercising used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(numbers != (cJSON *)0) + (int)(array_size == 3) + (int)(second_val == 20.0) + (int)(added_str != (cJSON *)0) + (int)(add_ok);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}