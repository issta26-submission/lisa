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
//<ID> 1579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root1 = NULL;
    cJSON *root2 = NULL;
    cJSON *num_item = NULL;
    char *unformatted = NULL;
    double num_value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root1 = cJSON_Parse("{ \"num\": 3.1415, \"text\": \"value\" }");
    root2 = cJSON_Parse("{\"num\":3.1415,\"text\":\"value\"}");

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root1, "num");
    num_value = cJSON_GetNumberValue(num_item);
    unformatted = cJSON_PrintUnformatted(root1);
    equal = cJSON_Compare(root1, root2, 1);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    // API sequence test completed successfully
    return 66;
}