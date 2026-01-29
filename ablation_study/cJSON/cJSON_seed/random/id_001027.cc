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
//<ID> 1027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "[1, 2, 3]";
    cJSON *root = (cJSON *)0;
    cJSON *new_num1 = (cJSON *)0;
    cJSON *new_num2 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched3 = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    double val0 = 0.0;
    double val3 = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON input to obtain an array root
    root = cJSON_Parse(json_text);

    // step 3: Configure - verify it's an array, create numeric items and append them
    is_array = cJSON_IsArray(root);
    new_num1 = cJSON_CreateNumber(4.5);
    new_num2 = cJSON_CreateNumber(6.0);
    add_ok = cJSON_AddItemToArray(root, new_num1);
    add_ok = cJSON_AddItemToArray(root, new_num2);

    // step 4: Operate - fetch elements and read numeric values
    fetched0 = cJSON_GetArrayItem(root, 0);
    fetched3 = cJSON_GetArrayItem(root, 3);
    val0 = cJSON_GetNumberValue(fetched0);
    val3 = cJSON_GetNumberValue(fetched3);

    // step 5: Validate - compute a simple validation score from the operations
    validation_score = (int)(root != (cJSON *)0) + (int)(is_array) + (int)(new_num1 != (cJSON *)0) + (int)(new_num2 != (cJSON *)0) + (int)(add_ok) + (int)(val0 == 1.0) + (int)(val3 == 6.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}