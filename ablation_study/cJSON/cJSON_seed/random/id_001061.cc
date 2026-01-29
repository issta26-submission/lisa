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
//<ID> 1061
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_answer = (cJSON *)0;
    cJSON *fetched_pi = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    char *printed = (char *)0;
    char *label_value = (char *)0;
    double answer_value = 0.0;
    double pi_value = 0.0;
    cJSON_bool add_ok_child_label = (cJSON_bool)0;
    cJSON_bool add_ok_child_pi = (cJSON_bool)0;
    cJSON_bool add_ok_root_child = (cJSON_bool)0;
    cJSON_bool add_ok_root_answer = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create objects and items
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_answer = cJSON_CreateNumber(42.0);
    num_pi = cJSON_CreateNumber(3.14);
    label = cJSON_CreateString("example-label");

    // step 3: Configure - attach items using both AddItemToObject and AddItemToObjectCS
    add_ok_child_pi = cJSON_AddItemToObject(child, "pi", num_pi);
    add_ok_child_label = cJSON_AddItemToObjectCS(child, "label", label);
    add_ok_root_child = cJSON_AddItemToObjectCS(root, "child", child);
    add_ok_root_answer = cJSON_AddItemToObject(root, "answer", num_answer);

    // step 4: Operate - fetch items, read numeric and string values, print and free JSON text
    fetched_child = cJSON_GetObjectItem(root, "child");
    fetched_answer = cJSON_GetObjectItem(root, "answer");
    fetched_pi = cJSON_GetObjectItem(fetched_child, "pi");
    fetched_label = cJSON_GetObjectItem(fetched_child, "label");
    answer_value = cJSON_GetNumberValue(fetched_answer);
    pi_value = cJSON_GetNumberValue(fetched_pi);
    label_value = cJSON_GetStringValue(fetched_label);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(fetched_child != (cJSON *)0) + (int)(fetched_answer != (cJSON *)0) + (int)(fetched_pi != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(answer_value == 42.0) + (int)(pi_value == 3.14) + (int)(label_value != (char *)0) + (int)(add_ok_child_pi) + (int)(add_ok_child_label) + (int)(add_ok_root_child) + (int)(add_ok_root_answer);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}