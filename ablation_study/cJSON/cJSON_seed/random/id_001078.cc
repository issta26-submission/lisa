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
//<ID> 1078
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    double num_value = 0.0;
    const char *str_value = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool add_num_ok = (cJSON_bool)0;
    cJSON_bool add_str_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root container, a number and a string
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(123.456);
    str = cJSON_CreateString("example");

    // step 3: Configure - attach the number and string to the root object
    add_num_ok = cJSON_AddItemToObject(root, "n", num);
    add_str_ok = cJSON_AddItemToObject(root, "s", str);

    // step 4: Operate - fetch the items back from the root and read their values
    fetched_num = cJSON_GetObjectItem(root, "n");
    fetched_str = cJSON_GetObjectItem(root, "s");
    num_value = cJSON_GetNumberValue(fetched_num);
    str_value = cJSON_GetStringValue(fetched_str);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(num_value > 100.0) + (int)(str_value != (const char *)0) + (int)(add_num_ok) + (int)(add_str_ok);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}