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
//<ID> 2064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *sum_item = NULL;
    char *unformatted = NULL;
    char *buffered = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num1 = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num2 = cJSON_AddNumberToObject(root, "two", 2.0);

    // step 3: Operate
    v1 = cJSON_GetNumberValue(num1);
    v2 = cJSON_GetNumberValue(num2);
    sum = v1 + v2;
    sum_item = cJSON_AddNumberToObject(root, "sum", sum);
    unformatted = cJSON_PrintUnformatted(root);
    buffered = cJSON_PrintBuffered(root, 64, 1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(sum_item != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(buffered != NULL);
    validation ^= (int)(v1 > 3.0);
    validation ^= (int)(v2 == 2.0);
    validation ^= (int)((sum - (v1 + v2)) == 0.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}