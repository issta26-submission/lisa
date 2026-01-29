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
//<ID> 1062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *metadata = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *answer_item = (cJSON *)0;
    cJSON *fetched_metadata = (cJSON *)0;
    cJSON *fetched_pi = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *fetched_answer = (cJSON *)0;
    const char *label_value = (const char *)0;
    char *printed = (char *)0;
    double pi_val = 0.0;
    double answer_val = 0.0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool add_ok4 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create objects, numbers and a string
    root = cJSON_CreateObject();
    metadata = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(3.14159);
    label_item = cJSON_CreateString("pi");
    answer_item = cJSON_CreateNumber(42.0);

    // step 3: Configure - attach items using both AddItemToObject and AddItemToObjectCS
    add_ok1 = cJSON_AddItemToObject(metadata, "pi_val", pi_item);
    add_ok2 = cJSON_AddItemToObjectCS(metadata, "label", label_item);
    add_ok3 = cJSON_AddItemToObject(root, "metadata", metadata);
    add_ok4 = cJSON_AddItemToObjectCS(root, "answer", answer_item);

    // step 4: Operate - fetch items, read numeric and string values, print JSON text
    fetched_metadata = cJSON_GetObjectItem(root, "metadata");
    fetched_pi = cJSON_GetObjectItem(fetched_metadata, "pi_val");
    fetched_label = cJSON_GetObjectItem(fetched_metadata, "label");
    fetched_answer = cJSON_GetObjectItem(root, "answer");
    pi_val = cJSON_GetNumberValue(fetched_pi);
    answer_val = cJSON_GetNumberValue(fetched_answer);
    label_value = cJSON_GetStringValue(fetched_label);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(metadata != (cJSON *)0)
                     + (int)(pi_item != (cJSON *)0)
                     + (int)(label_item != (cJSON *)0)
                     + (int)(add_ok1)
                     + (int)(add_ok2)
                     + (int)(add_ok3)
                     + (int)(add_ok4)
                     + (int)(fetched_pi != (cJSON *)0)
                     + (int)(fetched_label != (cJSON *)0)
                     + (int)(label_value != (const char *)0)
                     + (int)(pi_val > 3.1)
                     + (int)(answer_val == 42.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}