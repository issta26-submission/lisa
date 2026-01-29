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
//<ID> 2065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_pi = NULL;
    cJSON *num_answer = NULL;
    char *printed_unformatted = NULL;
    char *printed_buffered = NULL;
    double value_pi = 0.0;
    double value_answer = 0.0;
    int prebuffer = 64;
    cJSON_bool fmt = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Configure
    num_pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_answer = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate (printing and reading values)
    value_pi = cJSON_GetNumberValue(num_pi);
    value_answer = cJSON_GetNumberValue(num_answer);
    printed_unformatted = cJSON_PrintUnformatted(root);
    printed_buffered = cJSON_PrintBuffered(root, prebuffer, fmt);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_pi != NULL);
    validation ^= (int)(num_answer != NULL);
    validation ^= (int)(printed_unformatted != NULL);
    validation ^= (int)(printed_buffered != NULL);
    validation ^= (int)(value_pi > 3.0);
    validation ^= (int)(value_answer == 42.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(printed_buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}