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
//<ID> 946
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "count";
    const char *str_key = "title";
    const char *str_val = "example_title";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    double read_number = 0.0;
    char *read_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create number and string items and attach them to root
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString(str_val);
    add_ok1 = cJSON_AddItemToObject(root, num_key, num_item);
    add_ok2 = cJSON_AddItemToObjectCS(root, str_key, str_item);

    // step 4: Operate - read back values using getters
    read_number = cJSON_GetNumberValue(num_item);
    read_str = cJSON_GetStringValue((const cJSON *const)str_item);

    // step 5: Validate - produce a small aggregated score from results
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(read_number == 42.0) + (int)(read_str && read_str[0] != '\0');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}