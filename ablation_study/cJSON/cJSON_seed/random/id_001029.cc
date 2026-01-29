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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "[1, 2]";
    cJSON *root = (cJSON *)0;
    cJSON *existing0 = (cJSON *)0;
    cJSON *existing1 = (cJSON *)0;
    cJSON *newnum1 = (cJSON *)0;
    cJSON *newnum2 = (cJSON *)0;
    cJSON *fetched2 = (cJSON *)0;
    cJSON *fetched3 = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double val2 = 0.0;
    double val3 = 0.0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON text into a root (array)
    root = cJSON_Parse(json_text);

    // step 3: Configure - verify root is an array and create number items to add
    is_array = cJSON_IsArray(root);
    newnum1 = cJSON_CreateNumber(3.5);
    newnum2 = cJSON_CreateNumber(4.5);

    // step 4: Operate - add new numbers into the parsed array and fetch items
    add_ok1 = cJSON_AddItemToArray(root, newnum1);
    add_ok2 = cJSON_AddItemToArray(root, newnum2);
    existing0 = cJSON_GetArrayItem(root, 0);
    existing1 = cJSON_GetArrayItem(root, 1);
    fetched2 = cJSON_GetArrayItem(root, 2);
    fetched3 = cJSON_GetArrayItem(root, 3);
    val0 = cJSON_GetNumberValue(existing0);
    val1 = cJSON_GetNumberValue(existing1);
    val2 = cJSON_GetNumberValue(fetched2);
    val3 = cJSON_GetNumberValue(fetched3);

    // step 5: Validate - derive a small validation score exercising the used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(is_array) + (int)(newnum1 != (cJSON *)0) + (int)(newnum2 != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(val0 == 1.0) + (int)(val1 == 2.0) + (int)(val2 == 3.5) + (int)(val3 == 4.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}